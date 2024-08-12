#!/usr/bin/env perl


my $COLOR_MAX_DIST = sqrt(3*(256**2));

sub color_distance {
    my(@c1, @c2) = @_;
    return sqrt(
	($c1[0]-$c2[0])**2 +
	($c1[1]-$c2[1])**2 +
	($c1[2]-$c2[2])**2
    );
}


# abs_mean() returns a contrast value:
#   - The average of the absolute differences between each r,g and b triplet
#   - The result is, the closer in value of the r, g, b triplet components
#     the lower the contrast of that color
sub abs_mean {
    my (@a)=@_;
    my $sum = 0;  # sum of the absolute differences

    # iterate through r, g and b
    for(my $i=0; $i<$#a; $i++){
	   $sum += abs($a[$i]-$a[($i+1)%($#a+1)])
    }
    # average of the abs diffs
    return $sum/($#a+1);
}

my $c;
$"=",";
while(<>) {
    chop;
    my @F = split/\t/, $_;
    # print "@F\n";

    # The color name
    $F[1]=~s/\W+/_/mg;

    # Extract the r, g, b colors to an array
    my(@a)=$F[3]=~/\((\d+),(\d+),(\d+)\)/;

    # print "@a\t", abs_mean(@a), "\n";

    $c->{"@a"} = 0;
}

$"=" -- ";
my $distances, $pairs;
foreach $col1(sort keys %$c) {
    foreach $col2(sort keys %$c) {
	my $dist = int(color_distance($col1, $col2));
	$pairs->{$col1}->{$col2} = $dist;
	my $pair = [$col1, $col2];
	# print "pair $dist -- @$pair\n";
	push @{$distances->{$dist}}, $pair;
    }
}

my $square = scalar (keys %$c) **2;
print "area: $square\n";
print "*" x 100, "\n";
my @k = keys %$distances;
$"=", ";
# print "@k\n\n";
foreach $dist(sort {$a <=> $b} keys %$distances) {
    print "$dist\t", $#{$distances->{$dist}}, "\t", int($dist / $square * 10000) / 100, "\n";
}

# Sort the color array by contrast, low to high, write to stdout
#END{
#    foreach$k(sort { $a<=>$b } keys %$z){
#        foreach$row(@{$z->{$k}}){
#            print($row);
#        }
#    }
#}
#'
