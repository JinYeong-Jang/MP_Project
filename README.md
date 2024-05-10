convert.c 는 테스트용 코드
convert_clean.c 는 테스트용 코드에서 잡다한 코드 주석처리하고 RGB_RGB_RGB_RGB 순서대로 배치하여 compress
convert_clean_m2.c 는 RRR...RRR_GGG...GGG_BBB....BBB 순서대로 배치한 후 compress





----TROUBLESHOOTING----
0509    두 변환 코드 모두 C로는 정상 작동함을 확인했으나, keil자체 빌드가 안됌
0510    Error L6915E 발생
        https://community.arm.com/support-forums/f/keil-forum/40691/error-l6915e-use_no_semihosting-and-sys_exit =>안됌

        한글 주석 깨져서 영문으로 변환

        https://developer.arm.com/documentation/100073/0622/The-Arm-C-and-C---Libraries/Support-for-building-an-application-with-the-C-library/Indirect-semihosting-C-library-function-dependencies
        fputc()와 fgetc()가 라이브러리 종속성 문제로 printf와 scanf 종류로 재구현이 필요한것으로 보임.
        fputs() 또는 puts()의 경우 stream output 종류로 재구현이 필요한것으로 보임.

        https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/P-NUCLEO-WB55.Nucleo/Examples/I2C/I2C_TwoBoards_RestartAdvComIT/MDK-ARM/Serial.h
        retarget.c 와 serial.h를 불러와 알아서 재구현하게 만듦. => 컴파일 가능

        TODO
        keil에서 0x4000_0000부터 불러오기 때문에 메모리 단위로 여기서부터 계산하는 c 프로그램 생성
