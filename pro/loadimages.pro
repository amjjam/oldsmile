function loadImages,file

  openr,un,file,/get_lun,/compress
  
  tmp=readImage(un)
  img=[tmp,tmp]
  n=2l
  i=1l
  
  while not(eof(un)) do begin
     if i eq n then begin
        img=[img,img]
        n*=2l
     endif
     img[i]=readImage(un,template=tmp)
     i++
  endwhile
  
  free_lun,un

  i--

  return,img[0:i]
end
