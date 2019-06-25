import numpy as np
import cv2
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_alt.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye_tree_eyeglasses.xml')

cap = cv2.VideoCapture(0)

total = 0
flag = 0

while True:

    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = face_cascade.detectMultiScale(gray, 1.1, 2)
    for (x,y,w,h) in faces:
        cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = frame[y:y+h, x:x+w]
        eyes = eye_cascade.detectMultiScale(roi_gray)

        if eyes == ():
            cv2.putText(frame, "close the eyes", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, \
    					(70, 255, 0), 2)
            flag = 1
        else :
            cv2.putText(frame, "open the eyes", (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, \
                		(70, 255, 0), 2)
            if flag == 1:
                total = total + 1
                flag = 0

        for (ex,ey,ew,eh) in eyes:
            cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)

    # cv2.putText(frame, "Blinks: {}".format(total), (400, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, \
    #     (70, 255, 0), 2)
    cv2.imshow('frame',frame)
    # show the frame
    key = cv2.waitKey(1) & 0xFF

    # if the `q` key was pressed, break from the loop
    if key == ord("q"):
        break
