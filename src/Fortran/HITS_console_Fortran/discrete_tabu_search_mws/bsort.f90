subroutine bsort(np,nat,snat)
parameter (Tmax=50000, pmax=20)

integer np,nat(pmax),snat(pmax)

!locals
integer itmp
!integer bnat(pmax)

!save nat in snat and sort snat in increasing order
do i=1,np
   snat(i)=nat(i)
enddo

do i=1,np-1
   do j=i+1,np
      if (snat(i).gt.snat(j)) then
          itmp=snat(i)
          snat(i)=snat(j)
          snat(j)=itmp
      endif  
   enddo
enddo
return
end subroutine bsort