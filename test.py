#!/home/kk/anaconda3/envs/tensorflow/bin/python
# import keras
from socket import *
import cv2

# 固定相关信息
road_state = 0
# 创建套接字
udpSocket = socket(AF_INET, SOCK_DGRAM)
# 绑定本地相关信息，如果网络程序不稳定，系统会随机分配
# 同一主机可能有多个ip地址
bindAddr = ('', 8767)  # ' ' 未定义ip地址，自动确定本地的ip地址
udpSocket.bind(bindAddr)
# 准备接收方地址
sendAddr = ('127.28.55.3', 8080)  # ' ' 如果不写，也默认是本地的ip地址


def road_detect():
    global road_state
    while True:
        # a = cv2.namedWindow("dajai")
        img = cv2.imread('/home/kk/study/Keras/raw_dataset/0/1.jpg')
        cv2.imshow('dajai', img)

        road_state = 1
        send_data()
        a = cv2.waitKey(1000)
        if a == ord('q'):
            road_state = -1
            break


def send_data():
    global road_state
    global udpSocket
    sendData = str(road_state)
    # 发送数据指定的电脑上
    udpSocket.sendto(sendData.encode('utf-8'), sendAddr)  # 此处，是将sendData 转为utf-8 格式的编码 的一个一个字节的排布
    # udpSocket.sendto('\n'.encode("utf-8"),sendAddr)


if __name__ =="__main__":
    road_detect()
    udpSocket.close()
