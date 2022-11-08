import csv
import os
import pyautogui 
import time
import serial

height1=193.54
fat1,long1 = map(float,input('시작 위도와 경도를 입력하세요.').split())

PORT = 'COM9'
BaudRate = 9600
ARD= serial.Serial(PORT,BaudRate)
def Decode(A):
    A = A.decode('utf-8','ignore')
    A = str(A)
    if A[0]=='3':                      
        if (len(A)==26):                
            Ard1=float(A[0:11])
            Ard2=float(A[12:])
            result= [Ard1,Ard2]
            return result
        else : 
            print (" %d" %len(A))
            return False
    else :
        print ("")
        return False

def Ardread(): 
        if ARD.readable():
            LINE = ARD.readline()
            code=Decode(LINE) 
            return code
        else : 
            print("")

try:
    while (True):
            gps= Ardread()
            print(gps[0],gps[1])

except KeyboardInterrupt:
    print("")
    
####### 미션 플래너에서 사용가능한 파일 만들기
f=open("D:\\mission planner\\1.tsv",'w',encoding='utf-8',newline='')
wr=csv.writer(f,delimiter='\t')
wr.writerow(["QGC WPL 110"])
wr.writerow([0,1,0,16,0,0,0,0,fat1,long1,height1,1])
wr.writerow([1,0,3,22,0.00000000,0.00000000,0.00000000,0.00000000,0,0,0.000000,1])
wr.writerow([2,0,3,16,0.00000000,0.00000000,0.00000000,0.00000000,37.21944670,127.18441980	,3.000000,1])
wr.writerow([3,0,3,183,10,2000,0,0,0,0,3,1])
wr.writerow([4,0,3,21,0.00000000,0.00000000,0.00000000,0.00000000,gps[0],gps[1],3,1])
f.close()
oldname=os.path.join("D:\\mission planner\\","1.tsv")
newname=os.path.join("D:\\mission planner\\","1.txt")
os.rename(oldname,newname)

#### 미션 플래너 핫키
pyautogui.click(134,102) #계획 클릭
time.sleep(2)
pyautogui.click(2771,413) #파일로드 클릭
time.sleep(2)
pyautogui.click(435,353) #파일선택
time.sleep(2)
pyautogui.click(1060,947) #파일열기
time.sleep(2)
pyautogui.click(2703,644) #쓰기
time.sleep(2)

##### 파일 삭제(파일 위치 고정을 위해서)
# file_path = "D:\\mission planner\\1.txt"
# if os.path.exists(file_path):
#     os.remove(file_path)
   
