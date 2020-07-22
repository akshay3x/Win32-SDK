del main.obj 

del Application.exe

del resource.res


cl.exe /c /EHsc main.cpp

rc resource.rc 

link.exe /out:Application.exe main.obj resource.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

Application.exe