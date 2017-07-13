;
; Fit functional forms to the xems power law parameters in the magnetosheath
;

function a1_func,theta,phi,p

  a=p[0]
  b=p[1]
;  return,a+b*sin(theta)^8+c*sin(phi)^2
;  return,a+b*sin(alpha)^8+c*sin(beta)^8
;  return,a+(b*sin(alpha)^2+c*sin(beta)^2)^4
  return,a+b*sin(theta)^8
end


function b1_func,theta,phi,p
  alpha=p[0]
  beta=p[1]
;  return,alpha+beta*sin(theta)^2+gamma*sin(phi)^2
;  return,alpha+beta*sin(expalpha)^2+gamma*sin(expbeta)^2
  return,alpha+beta*sin(theta)^2
end


function f1_func,a,b,c,alpha,beta,gamma,theta,phi
  return,a1_func(a,b,c,theta,phi)*(r/10)^(-b1_func(alpha,gamma,beta,theta,phi))
end

function diff,p
  common diffcommon,func,theta,phi,data
  d=0d
  for i=0,n_elements(theta)-1 do begin
     for j=0,n_elements(phi)-1 do begin
        d+=abs(call_function(func,theta[i],phi[j],p)-data[j,i])
     endfor
  endfor
  
  return,d/n_elements(theta)/n_elements(phi)
end

common diffcommon,func,theta,phi,data
  
restore,'../adat/fitXemsSlopes.idl'

index=where(cos(2.*theta) gt -0.5)
theta=d.theta[index]
phi=d.phi
data=d.a1[*,index,3]
p=[3d-5,0]
xi=dblarr(2,2)
for i=0,1 do xi[i,i]=1d
func='a1_func'
powell,p,xi,1d-12,fmin,'diff',/double
print,p
print,fmin
a1params=p

p=[3,0]
data=d.b1[*,*,3]
for i=0,1 do xi[i,i]=1d
func='b1_func'
powell,p,xi,1d-12,fmin,'diff',/double
print,p
print,fmin
b1params=p

end
