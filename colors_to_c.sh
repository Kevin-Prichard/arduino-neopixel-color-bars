#!/usr/bin/env bash

# Convert Netscape named RGB decimal colors to c code format (from colors.txt)
# They look like: 	maroon	#800000	(128,0,0)
# Use: ./colors_to_c.sh < colors.txt > colors_by_contrast.txt
 
perl -F\\t -ane '
     # -F\\t = split lines on tabs
     # -ane = # invoke script once per line

BEGIN {
    $"=", ";
}

# abs_mean() returns a contrast value:
#   - The average of the absolute differences between each r,g and b triplet
#   - The result is, the closer in value of the r, g, b triplet components
#     the lower the contrast of that color
sub abs_mean(){my (@a)=@_;
    my $sum = 0;  # sum of the absolute differences

    # iterate through r, g and b
    for(my $i=0; $i<$#a; $i++){
	   $sum += abs($a[$i]-$a[($i+1)%($#a+1)])
    }
    # average of the abs diffs
    return $sum/($#a+1);
}

++$overall;

# The color name
$F[1]=~s/\W+/_/mg;

# Trim the trailing newline from the (red,green,blue) color values triplet
chop $F[3];

# Extract the r, g, b colors to an array
my(@a)=$F[3]=~/\((\d+),(\d+),(\d+)\)/;

# get the average of absolute differences between color components
my($m) = &abs_mean(@a);

# format as a call to static method Adafruit_NeoPixel.Color(),
# which becomes a list value
# print "mean $m\t@a\t", $m >= 27, "\n";
if($m >= 27) {
    ++$accepted;
    push @{$z->{int($m)}}, (
        sprintf(
            "\t/* %3d */  pixels.Color(%3d, %3d, %3d),  /* %s */\n",
            $m, $a[0], $a[1], $a[2], $F[1]),
        ($a[0] << 16) + ($a[1] << 8) + $a[0]
    );
    # print "        ", ($a[0] << 16) + ($a[1] << 8) + $a[0], ";\n";

} else {
    ++$rejected;
}

# Sort the color array by contrast, low to high, write to stdout
END{
    print "Total lines: $overall, accepted: $accepted, rejected: $rejected\n";
    my $ptr = 0;
    my $colorIndex;
    $"="\n****\n";
    foreach$k(sort { $a<=>$b } keys %$z) {
        # print "$k\t@{$z->{$k}}\n";
	my $inner = 0;
        foreach$row($z->{$k}) {
	    ++$inner;
            # print "hereee @{$row}\n";
            print "row: @$row\n";
            $colorIndex->{$$row[1]} = $ptr++;
        }
	print "inner: $inner\n";
    }

    print "how many: $ptr\n";

    foreach $comp (("Red", 0x00FF0000), ("Green", 0x0000FF00), ("Blue", 0x000000FF)) {
        print "unsigned char colorIndex${comp[0]}[] = {\n";
    	foreach $color(sort { $a & $comp[1] <=> $b & $comp[1] } keys %$colorIndex) {
            print "    $colorIndex->{$color},\n"; 
        }
        print "};  /* End of colorIndex${comp[0]}[] */\n\n";
    }
}
'
