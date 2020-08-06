#!/bin/bash

#theta: 22-38, phi: 0-90
#theta: 22-33, phi: 0-90
macname=$1

for((i=1;i<=10;i++));
do
    for((j=1;j<=20;j++));
    do
        #theta=$[22+$i*(38-22)/10];
        theta=`awk -v i=$i 'BEGIN {printf("%.3f",22+i*(38-22)/10)}'`
        phi=`awk -v j=$j 'BEGIN {printf("%.3f",j*(89-1)/20)}'`
        #phi=$[$j*89/20];
         
        x=`echo | gawk -v theta=$theta -v phi=$phi '{printf("%.3f",sin(theta/180*3.1415926)*cos(phi/180*3.14));}'`
        y=`echo | gawk -v theta=$theta -v phi=$phi '{printf("%.3f",sin(theta/180*3.1415926)*sin(phi/180*3.14));}'`
        z=`echo | gawk -v theta=$theta '{printf("%.3f", cos(theta/180*3.14));}'`
        #echo " theta & phi is: $theta, $phi"
        #echo " x & y & z is: $x, $y, $z"
        string="/gps/direction $x $y $z"
        constantstr="/run/beamOn 1000"
        echo $string
        echo $constantstr
        echo $string >> $macname
        echo $constantstr >> $macname

    done
done

