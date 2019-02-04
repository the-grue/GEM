clean:
 del *.obj
 del *.sym
 del *.lst
 del *.map
 del gemvdi.exe 


 

 rasm86 entry $szpz 

 rasm86 escape $szpz 

 rasm86 control $szpz 
 
 rasm86 fonts $szpz  

 rasm86 headers $szpz 

 rasm86 driver $szpz 

 rasm86 utility $szpz 

 rasm86 dosutil $szpz  

 rasm86 data $szpz  

 rasm86 loader $szpz 

 rasm86 command $szpz  

 rasm86 decode $szpz  

 rasm86 stack $szpz 
 
 link86 gemvdi[nop]=gemvdi[i]  
