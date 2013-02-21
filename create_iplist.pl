#!/usr/bin/perl

# 合計15000件のIPアドレスリストを作成する
# 第一～第三オクテットはランダムにチョイスし、
# 第四オクテットは、第一～第三オクテットの組み合わせに対し、
# 100個の値を抽出する

use strict;
use warnings;

my @iplist = ();
my %created_ip_1to3 = ();

while ( @iplist < 15000 ) {
	# 第一～第三オクテットを決定
	my $ipstr_1to3 = "";
	for ( 1..3 ) {
		$ipstr_1to3 .= int(rand(254)+1) . '.';
	}
	# 生成済みの第一～第三オクテットの組みだった場合は作り直す
	next if exists $created_ip_1to3{$ipstr_1to3};
	$created_ip_1to3{$ipstr_1to3} = "exists";
	
	# 第四オクテットをランダムに100個生成し、IPアドレスに追加
	my %create_iplist = ();
	while ( keys(%create_iplist) < 100 ) {
		my $ipaddr = $ipstr_1to3 . int(rand(254) + 1);
		$create_iplist{$ipaddr} = "exists" unless exists $create_iplist{$ipaddr};
	}
	push(@iplist, keys(%create_iplist));
}

@iplist = sort(@iplist);

print join("\n", @iplist);
print "\n";
