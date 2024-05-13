- convert.c 는 테스트용 코드
- convert_clean.c 는 테스트용 코드에서 잡다한 코드 주석처리하고 RGB_RGB_RGB_RGB 순서대로 배치하여 compress
- convert_clean_m2.c 는 RRR...RRR_GGG...GGG_BBB....BBB 순서대로 배치한 후 compress

---
# TROUBLESHOOTING

## 0509   
두 변환 코드 모두 C로는 정상 작동함을 확인했으나, keil자체 빌드가 안됌

## 0510    
Error L6915E 발생
https://community.arm.com/support-forums/f/keil-forum/40691/error-l6915e-use_no_semihosting-and-sys_exit =>안됌

한글 주석 깨져서 영문으로 변환

https://developer.arm.com/documentation/100073/0622/The-Arm-C-and-C---Libraries/Support-for-building-an-application-with-the-C-library/Indirect-semihosting-C-library-function-dependencies
fputc()와 fgetc()가 라이브러리 종속성 문제로 printf와 scanf 종류로 재구현이 필요한것으로 보임.
fputs() 또는 puts()의 경우 stream output 종류로 재구현이 필요한것으로 보임.

https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/P-NUCLEO-WB55.Nucleo/Examples/I2C/I2C_TwoBoards_RestartAdvComIT/MDK-ARM/Serial.h
retarget.c 와 serial.h를 불러와 알아서 재구현하게 만듦. => 컴파일 가능

### TODO
keil에서 0x4000_0000부터 불러오기 때문에 메모리 단위로 여기서부터 계산하는 c 프로그램 생성

## 0513
keil에서 이미지 파일을 직접 불러오도록 프로젝트에 png파일 포함.
output 파일을 지정해줘야 한다 오류가 뜨지만, 의도한대로 작동하지 않고 출력파일은 집어넣은 이미지 파일을 그대로 hex형태로 변환해서 띄워주기만하고 빌드는 불가함.

이에 따라 keil에서 fread를 정상 작동시킬수 없어 프로그램이 항상 예외처리로 빠지고, 정상적으로 이미지를 처리할 수 없음

### 이후 수정과정
1. 이전 TODO에서 언급한대로 0x4000_0000부터 메모리 단위로 구현하는 C 프로그램 생성. 전통적인 C프로그래밍 방식은 아니지만(메모리에 직접 접근하게 되므로) 현재로서는 읽어온 파일을 처리할 다른 방법을 찾지못함
2. 하위 반복문들까지 모두 새로운 방식에 맞춰 작성한 후 실행했으나, 배열에서 사용하는 주소를 보고 새로 Memory map을 해줘야 쓰기가 가능했음. 배열은 총 3개(rgba, rgb, rgbcomp)가 사용되므로 그때마다 새로 쓰기권한을 부여해야 했음. 쓰기권한 오류가 떠서 쓰기 권한을 지정할때까지 첫 데이터는 손실되었으나 쓰기 권한을 부여한 이후 처리된 나머지 데이터는 모두 정상적으로 기존의 C코드와 동일하게 결과를 도출시킴.
3. 매번 쓰기 권한을 유동적인 곳에 지정해야 하므로, ARM은 임베디드 시스템에 주로 사용되는 것을 고려해 이후 세 배열도 모두 배열 포인터를 사용한 특정 메모리 범위에서 작동하도록 수정. 이 또한 전통적인 C프로그래밍 방식은 아니나 이미지를 load하고 데이터를 처리하기 전 일정한 곳에 쓰기 권한을 부여해 이미지를 처리하게 설계함.
4. return(0) 을 예제 프로젝트를 참고하여 _sys_exit(0)으로 바꿈. 오류코드를 뱉어내는 loop에서 프로그램이 정상적으로 종료됨. bmask도 따로 구분하지 않고 mask변수 하나로 rgbcomp를 구현해도 정상 작동함.
++keil 작업 영역을 현재 branch 작업영역 내부로 이동, keil 작업내용도 기록될 것임.

2번째 알고리즘에 대해서도 테스트 후 완벽 동작 확인, convert_keil_m2.c에 완성된 코드 저장

### TODO
- 두 알고리즘의 속도 차이 비교
- 두 알고리즘 호환성 최적화(이미지 크기 변동 등)
- Func 1~3 함수로 포함시켜 두 알고리즘에 대해 각각 실행