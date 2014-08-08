#!/usr/bin/python3

import PastecLib

c = PastecLib.PastecConnection()

c.clearIndex()

for i in range(1, 24):
    c.indexImageFile(i, "/home/magsoft/Bureau/test/%d.jpg" % i)
#c.indexImageFile(1, "/home/magsoft/Bureau/test2.png")

#c.removeImage(22)

print(c.imageQueryFile("/home/magsoft/Bureau/test/test4.jpg"))

c.writeIndex()
