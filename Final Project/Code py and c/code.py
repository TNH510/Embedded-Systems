import RPi.GPIO as GPIO
import time
import rospy
from std_msgs.msg import Float64
import math
import os

BT1 = 15
BT5 = 16
BT6 = 18
BT4 = 22
BT2 = 40
BT3 = 32

fifo = "mypipe"

# Check if the named pipe exists, if not, create it
if not os.path.exists(fifo):
    os.mkfifo(fifo)

# Set up GPIO mode
GPIO.setmode(GPIO.BOARD)

# Set up GPIO pin
GPIO.setup(BT1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BT2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BT3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BT4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BT5, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BT6, GPIO.IN, pull_up_down=GPIO.PUD_UP)

#Init Velocity
v_level = 0
slow_speed_up = 1
slow_turn = 1
data_pub_L = 0
data_pub_R = 0
temp_v = 0
last_state = 'nothing'
 
def vel(v_level):
    return 2.0*v_level
    
while True:
    pub2 = rospy.Publisher('/my_diffbot/joint1_velocity_controller/command', Float64, queue_size=10)
    pub1 = rospy.Publisher('/my_diffbot/joint2_velocity_controller/command', Float64, queue_size=10)                           
    rospy.init_node('talker', anonymous=True)
    
    #Xu li data nhan duoc
    if (GPIO.input(BT2) == 0 or GPIO.input(BT3) == 0): #Tien
        if slow_speed_up == 1:
            temp_v = 0.0
            while(temp_v < vel(v_level)):
                #Go ahead
                if (GPIO.input(BT2) == 0 and GPIO.input(BT1) == 1 and GPIO.input(BT4) == 1):
                    data_pub_L = -temp_v
                    data_pub_R = temp_v
                    last_state = 'A'
                #Phai + Thang
                elif (GPIO.input(BT2) == 0 and GPIO.input(BT4) == 0): 
                    data_pub_L = -vel(temp_v)
                    data_pub_R = -vel(temp_v) 
                    last_state = 'R'                 
                #Trai + Thang        
                elif (GPIO.input(BT2) == 0 and GPIO.input(BT1) == 0): 
                    data_pub_L = vel(temp_v)
                    data_pub_R = vel(temp_v)
                    last_state = 'L' 
                #Go back
                elif (GPIO.input(BT3) == 0 and GPIO.input(BT1) == 1 and GPIO.input(BT4) == 1):
                    data_pub_L = temp_v
                    data_pub_R = -temp_v
                    last_state = 'B'
                #Trai + Go back       
                elif (GPIO.input(BT3) == 0 and GPIO.input(BT1) == 0): 
                    data_pub_L = vel(temp_v)
                    data_pub_R = vel(temp_v)
                    last_state = 'L'
                #Phai + Go back       
                elif (GPIO.input(BT3) == 0 and GPIO.input(BT1) == 0): 
                    data_pub_L = vel(temp_v)
                    data_pub_R = vel(temp_v)
                    last_state = 'R'
                
                #Increase vel   
                temp_v = temp_v + vel(v_level)/8
                time.sleep(0.08)
            slow_speed_up = 0
        else:
            #maximum velocity 
            if (GPIO.input(BT2) == 0 and GPIO.input(BT1) == 1 and GPIO.input(BT4) == 1): #Tien
                data_pub_L = -vel(v_level)
                data_pub_R = vel(v_level)
                slow_turn = 1
                
            if (GPIO.input(BT3) == 0 and GPIO.input(BT1) == 1 and GPIO.input(BT4) == 1): #Lui
                data_pub_L = vel(v_level)
                data_pub_R = -vel(v_level)
                slow_turn = 1
                
            if (GPIO.input(BT2) == 0 and GPIO.input(BT4) == 0 and GPIO.input(BT1) == 1): #Phai + Thang
                if (slow_turn == 1):  
                    temp_v = 0
                    while(temp_v < vel(v_level)):
                        data_pub_L = -vel(temp_v)
                        data_pub_R = -vel(temp_v) 
                        #Increase vel   
                        temp_v = temp_v + vel(v_level)/6
                        time.sleep(0.03)
                    slow_turn = 0
                else:
                    data_pub_L = -vel(v_level)
                    data_pub_R = -vel(v_level)              
            if (GPIO.input(BT2) == 0 and GPIO.input(BT1) == 0 and GPIO.input(BT4) == 1): #Trai + Thang
                if (slow_turn == 1):  
                    temp_v = 0
                    while(temp_v < vel(v_level)):
                        data_pub_L = vel(temp_v)
                        data_pub_R = vel(temp_v) 
                        #Increase vel   
                        temp_v = temp_v + vel(v_level)/6
                        time.sleep(0.03)
                    slow_turn = 0
                else:
                    data_pub_L = vel(v_level)
                    data_pub_R = vel(v_level)  
                    
            if (GPIO.input(BT3) == 0 and GPIO.input(BT4) == 0 and GPIO.input(BT1) == 1): #Phai + Lui
                if (slow_turn == 1):  
                    temp_v = 0
                    while(temp_v < vel(v_level)):
                        data_pub_L = -vel(temp_v)
                        data_pub_R = -vel(temp_v) 
                        #Increase vel   
                        temp_v = temp_v + vel(v_level)/6
                        time.sleep(0.03)
                    slow_turn = 0
                else:
                    data_pub_L = -vel(v_level)
                    data_pub_R = -vel(v_level)              
            if (GPIO.input(BT3) == 0 and GPIO.input(BT1) == 0 and GPIO.input(BT4) == 1): #Trai + Lui
                if (slow_turn == 1):  
                    temp_v = 0
                    while(temp_v < vel(v_level)):
                        data_pub_L = vel(temp_v)
                        data_pub_R = vel(temp_v) 
                        #Increase vel   
                        temp_v = temp_v + vel(v_level)/6
                        time.sleep(0.03)
                    slow_turn = 0
                else:
                    data_pub_L = vel(v_level)
                    data_pub_R = vel(v_level)
    
    elif (GPIO.input(BT4) == 0 and GPIO.input(BT2) == 1 and GPIO.input(BT3) == 1): #Phai
        data_pub_L = -vel(v_level+1)
        data_pub_R = -vel(v_level+1)
            
        
    elif (GPIO.input(BT1) == 0 and GPIO.input(BT2) == 1 and GPIO.input(BT3) == 1): #Trai
        data_pub_L = vel(v_level + 1)
        data_pub_R = vel(v_level + 1)
     
    elif (GPIO.input(BT5) == 0):
        v_level = v_level + 1
        if (v_level > 15):
            v_level = 15
        
    elif (GPIO.input(BT6) == 0):
        v_level = v_level - 1
        if (v_level < 1):
            v_level = 1
    
    else:
        slow_speed_up = 1
        while (temp_v > vel(v_level)/5):
            temp_v = temp_v - vel(v_level)/5
            if (last_state == 'A'):
                data_pub_L = -temp_v
                data_pub_R = temp_v
            elif (last_state == 'B'):
                data_pub_L = temp_v
                data_pub_R = -temp_v
            
            pub1.publish(data_pub_L)
            pub2.publish(data_pub_R)
            time.sleep(0.1)
            
        print("Stop oke")
        temp_v = 0.0
        data_pub_L = 0
        data_pub_R = 0
        
    #Publish data
    pub1.publish(data_pub_L)
    pub2.publish(data_pub_R)
    time.sleep(0.1)
    
    with open(fifo, "w") as pipe:
        pipe.write(str(vel(v_level)))

        

