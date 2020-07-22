del ColoredText.obj 

del ColoredText.exe

del resource.res


cl.exe /c /EHsc ColoredText.cpp

rc resource.rc 

link.exe ColoredText.obj resource.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

ColoredText.exe