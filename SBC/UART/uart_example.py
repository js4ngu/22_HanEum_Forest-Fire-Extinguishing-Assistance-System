import time
from turtle import distance          # 타임모듈 불러오기
import serial  # pyserial 모듈 불러오기


ser = serial.Serial(                    # serial 객체 생성
    port='/dev/ttyS0',            # 시리얼통신에 사용할 포트
    baudrate=9600,                # 통신속도 지정
    parity=serial.PARITY_NONE,      # 패리티 비트 설정방식
    stopbits=serial.STOPBITS_ONE,   # 스톱비트 지정
    bytesize=serial.EIGHTBITS,      # 데이터 비트수 지정
    timeout=1                       # 타임아웃 설정
)


while True:
    data = ser.readine()
    '''
    GPS = [data[0:14], data[16:30], data[31:45]]
    IMU = [data[47:56], data[58:67], data[68:77]]
    Lidar = data[79:83]
    '''
    print(data)

    """
    degree = input('what degree do you want for servo?\n')
    if degree.isdigit():
        ser.write(degree.encode())
    else:
        print("you have to pass a number")
    """
