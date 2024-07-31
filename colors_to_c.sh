#!/usr/bin/env bash

# Use: ./colors_to_c.sh < colors.txt > colors_by_contrast.txt

perl -F\\t -ane '
sub m(){my (@a)=@_;
    my $t=0;
    for(my $i=0; $i<$#a; $i++){
    	   # print "abs($a[$i]-$a[($i+1)%($#a+1)]) = ".(abs($a[$i]-$a[($i+1)%($#a+1)]))."\n";
	   $t += abs($a[$i]-$a[($i+1)%($#a+1)])
    }
    return $t/($#a+1);
}

$F[1]=~s/\W+/_/mg;
chop $F[3];
my(@a)=$F[3]=~/\((\d+),(\d+),(\d+)\)/;
my($m)=&m(@a);
push @{$z->{int($m)}}, sprintf("\t/* %3d */  ", $m).
      "pixels.Color$F[3],  /* $F[1] */\n";
END{
    foreach$k(sort { $a<=>$b } keys %$z){
        foreach$row(@{$z->{$k}}){
            print($row);
        }
    }
}
'
    
