import serial
PORT = 'COM10'
BaudRate = 9600
ARD= serial.Serial(PORT,BaudRate)

def Decode(A):
    A = A.decode('utf-8','ignore')
    A = str(A)
    if A[0]=='Q':                      
        if (len(A)==9):                
            Ard1=int(A[1:3])
            Ard2=int(A[3:8])
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
            print(code)
            return code
        else : 
            print("")

def main():
    while (True):
        Ardread()

main()