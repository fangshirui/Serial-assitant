from keras.models import load_model

import cv2

road_class = load_model('road_classfier.model.h5')
while True:
    frame = cv2.imread('./dataset/ordinary_road/15.jpg')
    frame = cv2.resize(frame, (32, 32))
    image = frame.reshape((1, 32, 32, 3))

    numberId = road_class.predict_classes(image)
    print(numberId)
    cv2.imshow("df",frame)
    k = cv2.waitKey(100)
    if k & 0xFF == ord('q'):
        break

