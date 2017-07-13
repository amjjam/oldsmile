function readImage,un,template=tmp
  
  n=0l & m=0l 
  readu,un,n,m
  
  if not(keyword_set(tmp)) then begin
     tmp={n:n,m:m,roll:0d,pitch:0d,yaw:0d,x:0d,y:0d,z:0d,$
          yr:0l,mo:0l,dy:0l,hr:0l,mn:0l,se:0l,ns:0l,$
          data:dblarr(n,m)}
  endif
  
  if not(n eq tmp.n and m eq tmp.m) then begin
     print,"Error: dimensions of template does not match dimensions read "+$
           "from file"
     stop
  endif
  
  roll=0d & pitch=0d & yaw=0d
  readu,un,roll,pitch,yaw

  tmp.roll=roll
  tmp.pitch=pitch
  tmp.yaw=yaw

  x=0d & y=0d & z=0d
  readu,un,x,y,z
  tmp.x=x
  tmp.y=y
  tmp.z=z

  yr=0l & mo=0l & dy=0l & hr=0l & mn=0l & se=0l & ns=0ll
  readu,un,yr,mo,dy,hr,mn,se,ns
  
  tmp.yr=yr
  tmp.mo=mo
  tmp.dy=dy
  tmp.hr=hr
  tmp.mn=mn
  tmp.se=se
  tmp.ns=ns

  data=dblarr(m,n)
  print,n,m
  readu,un,data

  tmp.data=transpose(data)

  return,tmp
end
