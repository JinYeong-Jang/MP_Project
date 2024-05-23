- convert.c 는 테스트용 코드
- convert_clean.c 는 테스트용 코드에서 잡다한 코드 주석처리하고 RGB_RGB_RGB_RGB 순서대로 배치하여 compress
- convert_clean_m2.c 는 RRR...RRR_GGG...GGG_BBB....BBB 순서대로 배치한 후 compress
- PNG구조 공부: https://hojin3777.notion.site/22ae61fe3f6f47469b55f6b17820b430?pvs=74
- LodePNG: https://lodev.org/lodepng/
- 지금까지 작동한 한줄기 빛: https://cplusplus.com/forum/beginner/267364/
- stb_image.h : https://github.com/nothings/stb/blob/master/stb_image.h
- stb_image_write.h : https://github.com/nothings/stb/blob/master/stb_image_write.h
- Keil hex in python : https://stackoverflow.com/questions/66589411/load-hex-file-with-intelhex-format-in-python


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
4. return(0) 을 예제 프로젝트를 참고하여 _sys_exit(0)으로 바꿈. 오류코드를 뱉어내는 loop에서 프로그램이 정상적으로 종료됨. bmask도 따로 구분하지 않고 mask변수 하나로 rgbcomp를 구현해도 정상 작동함.\
++keil 작업 영역을 현재 branch 작업영역 내부로 이동, keil 작업내용도 기록될 것임.

2번째 알고리즘에 대해서도 테스트 후 완벽 동작 확인, convert_keil_m2.c에 완성된 코드 저장

### TODO
- 두 알고리즘의 속도 차이 비교
- 두 알고리즘 호환성 최적화(이미지 크기 변동 등)
- Func 1~3 함수로 포함시켜 두 알고리즘에 대해 각각 실행

## 0514회의
실행시간\
&emsp;&emsp;&emsp;all(load)&emsp;&emsp;&emsp;&emsp;&emsp;comp\
case1&emsp;49.156ms(34.817)&emsp;15\
case2&emsp;74.244ms(40.449)&emsp;34\
수업시간에 들은 것과 달리 두번째 case가 더 느렸음. => 배열의 특성을 활용하지 못한 알고리즘으로, 레지스터 길이 단위로 처리량이 많아진 알고리즘으로 개선 필요.

## 0519
두번째 저장 배열에서 4바이트 단위로 연산하도록 코드 수정, for문을 포함하여 돌리는게 연산에 시간이 약간 더 걸려서 while문 3개로 배치.
- case2에서\
for 없이 58.497\
for 돌리면 59.777

그 결과 이전과 달리 압축 시간은 확 줄었는데 오히려 load가 10ms정도 더걸림(???)
- load에 51.201ms\
58.500ms에 종료\
comp에 7ms

### TODO
- Load 알고리즘 최적화 방법 생각해내기
- 압축 알고리즘은 얼추 정답? 같지만 더 다듬어보기

## 0520
두번째 방식 전체적인 코드 수정, 첫번째 방식과 비교해보니 같은 코드여도 뒤에 따라오는 코드에 따라 load부분에서 어셈 명령어가 달라짐.

사용하는 변수를 줄이고 코드를 다듬으니 어제보다 load에 약 6ms정도 덜 걸리고, 압축은 여전히 약 7ms정도 소요됨

load에서 약 10ms정도 더 걸리나 압축은 2배정도 더 빠름 => 각 Image converting 함수에 대해서 더 최적화가 가능함을 확인함.

### Load 알고리즘 수정
RGBRGBRGB order와 동일한 원리로 반복문 1개로 load하도록 수정,\
load에 33.281ms 소요, 그 외 자잘한 코드 최적화로(변수 초기와 위치 변경 등), load시간도 case1보다 1ms더 빠름.

case1도 비슷한 방식으로 최적화, 그러나 소요시간은 차이 없음.

압축은 동일하게 7ms소요

## 0521
회의 도중 grayscale 및 binary 또한 4바이트 단위로 처리하여 최적화할수 있는 알고리즘 제의
<img width="400" alt="KakaoTalk_20240521_155945849" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/143b9324-8f82-4486-bbaf-232a63fb3de4">

또한 PNG 파일 decode 및 encode 가능한 라이브러리 찾아냄(!!!)
<img width="400" alt="KakaoTalk_20240521_220747035" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/b5a66e1c-07e6-493a-9d48-92765f971604">

첫번째 픽셀 및 마지막 4칸의 픽셀 정보와 출력한 HEX파일이 일치함 확인
<img width="500" alt="KakaoTalk_20240521_222627920" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/28cd134d-f954-467b-ba5a-49dab0a1ccab">

이를 바탕으로 R정보만 255로 고정시키는 간단한 테스트 진행
<img width="371" alt="KakaoTalk_20240521_224842634" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/56634ec8-0b53-4c20-ae13-71a547c19f0c">
<img width="371" alt="KakaoTalk_20240521_224849615" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/09f07874-2e83-41a1-8b6f-a219a8336d24">

-> 모두 정상작동!

### TODO
- Keil 위에서 돌아가는지 확인
- Keil 위에서 돌아가지 않는 경우, VS에서 HEX 파일만 추출하여 Keil에서 데이터만 처리 후 출력된 결과물로 다시 PNG파일 변환

## 0522
간단히 픽셀당 1바이트 크기의 Grayscale이미지를 생성하는 테스트 진행\
<img width="400" alt="image" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/234773fe-1bd1-4998-ac30-a1d65945a50d">\
1바이트 크기의 Grayscale은 동작하나, 문제는 라이브러리가 1바이트 RGB는 지원하지 않음(func1에서 필요). 

이미지를 Hex 파일로 내보내도, HEX veiwer에서는 정상적으로 데이터를 볼 수 있으나 intelHex 형식이 아니라서 keil에서 정상적으로 불러오지 못함

반대로 keil에서 내보낸 파일은 intelHex 형식이어서 다시 c에서 이미지로 변환하려면 intelHEX파일을 배열에 집어넣는 과정이 필요함

https://github.com/AhmedYousriSobhi/Hex-To-array-txt\
회의중 hex를 텍스트 배열로 바꿔주는 코드를 찾아 테스트
<img width="400" alt="스크린샷 2024-05-22 173554" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/8a7a638c-22a8-41d1-9c57-590a5fb5078f">

그러나 이 코드를 이해해서 필요에 맞게 수정한 후, 배열에 넣어 이미지로 변환하는 과정이 추가로 필요했고, 또한 구해온 라이브러리에서 1바이트 RGB를 지원하지 않아 상대적으로 쉬운 파이썬을 활용하여 구현하기로 결정함

C에서 약 8000줄의 png변환 헤더와 load, write명령어 등 복잡한 과정이 필요했던것과 달리 단 5줄의 코드로 픽셀 정보를 얻을 수 있었음\
<img width="400" alt="pythonTTALKKAK" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/cd32bd25-c826-46f3-9216-770f4f223580">

## 0523
Keil에서 받은 output을 활용하여 파이썬에서 이를 8-bit RGB로 출력해주는 코드 생성\
PNG파일 특성상 과제에서 주어진 이미지 형식을 지원하지 않아 Keil에서 받은 데이터를 토대로 Image palette를 만들고, 다시 각 픽셀 데이터를 팔레트와 매칭시켜준 후 이미지에 픽셀 데이터와 팔레트를 집어넣는 식으로 구현.\
배열의 형태가 비효율적으로 바뀌지만 팀원들의 이해를 위해 배열의 shape이 바뀌는 코드들을 그대로 둠.\
<img width="400" alt="compRGB" src="https://github.com/JinYeong-Jang/MP_Project/assets/71000630/3d5d08f0-37b2-4a0d-ac83-5bb2e816ee9b">

### TODO
- func 2, 3에 대해서도 출력하는 keil 코드 수정 및 파이썬 코드 생성
- keil에서 Grayscale생성 시 /3을 어셈블리에서 제대로 처리하지 못해(output을 생성하긴 하나 /3에서 inf. loop가 걸림) max min 알고리즘을 사용하되 4바이트 연산 등 효율적인 연산이 가능한지 확인(생성한 output으론 이미지 생성 완료)
- 현준님이 제공한 binary(*3으로 구한 binary, max min으로 구한 binary) 파일 둘 다 변환(완료)
- 코드 다듬고 보고서 작성