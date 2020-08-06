
#!/bin/bash

#z=1415mm
#r=620 710 800 890 980
#phi=5 15 25 35 45
#x= r*cos

#theta: 22-38, phi: 0-90
#theta: 22-33, phi: 0-90

macname=$1


for P in 1.0 1.2 1.4 1.6 1.8 2.0
#for P in 1.0 1.2 
do
#for r in 620 710
for r in 620 710 800 890 980
do
    #for j in 5 15
    for j in 5 15 25 35 45
    do
        #theta=$[22+$i*(38-22)/10];
        z=1415
        theta=`awk -v r=$r -v z=$z 'BEGIN {printf("%.3f",atan2(r,z))}'`
        phi=`awk -v j=$j 'BEGIN {printf("%.3f",j/180*3.1415926)}'`
        #phi=$[$j*89/20];
         
        x=`echo | awk -v r=$r -v phi=$phi '{printf("%.3f",r*cos(phi));}'`
        y=`echo | awk -v r=$r -v phi=$phi '{printf("%.3f",r*sin(phi));}'`
        echo " theta & phi is: $theta, $phi"
        echo " x & y & z is: $x, $y, $z"
        themac="runP${P}r${r}phi${j}.mac"
        echo $themac
        cp $macname $themac
        sed -i "" "/direction/s/x/$x/g" $themac
        sed -i "" "/direction/s/y/$y/g" $themac
        sed -i "" "/energy/s/P/$P/g" $themac
        ./build/DIRC $themac build/runP${P}r${r}phi${j}.root >build/runP${P}r${r}phi${j}.log

        #string="/gps/direction $x $y $z"
        #constantstr="/run/beamOn 1000"
        #echo $string
        #echo $constantstr
        #echo $string >> $macname
        #echo $constantstr >> $macname

    done
done
done

