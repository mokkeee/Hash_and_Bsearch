#!/usr/bin/perl

# ハッシュサイズ(N)とハッシュ係数(k)に従い、
# 指定されたIPアドレスファイルの各IPアドレスを
# ハッシュ値に変換し、同一ハッシュ値に対するシノニム数と
# 生成されたハッシュ値のバリエーション(数)を表示する
# IPアドレスファイルは、IPv4アドレスのみが記載されたファイルを指定するものとする
# (create_iplist.plにより生成したIPアドレスファイルを想定)
# ハッシュ値は次の計算により算出している
#  (IPv4アドレス値(ネットワークバイトオーダ) * ハッシュ係数) % ハッシュサイズ

use IO::File;

use strict;
use warnings;

unless ( scalar(@ARGV) eq 3 ) {
	print "Usage : check_hash.pl Hashサイズ ハッシュ係数 IPアドレスリストファイル名\n";
	exit(1);
}

my ($hash_size, $hash_coefficient, $iplist_file) = @ARGV;

# ファイルのIPアドレスリスト読み込み
my $fh;
open($fh, "<", $iplist_file) or die;
my @iplist = <$fh>;
close($fh);

my %hash_synonym = ();		# 各ハッシュ値に対するシノニム数を扱う
my $synonym_max = 1;		# シノニム深さ最大値
my $hashval_count = 0;		# 生成されたハッシュの数
# 各IPアドレスに対してハッシュ値を求め、シノニムの深さを算出
foreach my $ipaddr (@iplist) {
	# ハッシュ値計算
	# ハッシュ値は、IPアドレスの値に係数を掛け、ハッシュのサイズで除算した余り
	my $hashval = int((get_ipvalue($ipaddr) * $hash_coefficient) % $hash_size);
	
	# シノニムあり
	if (exists($hash_synonym{$hashval})) {
		my $count = $hash_synonym{$hashval};
		$hash_synonym{$hashval} = ++$count;
		
		# シノニムの深さが最大の場合は、深さの値を更新
		$synonym_max = $count if $count > $synonym_max;
	} else {
		$hash_synonym{$hashval} = 1;
		$hashval_count++;
	}
}

# 各ハッシュ値に対するシノニムの数を表示
#while ( my($hashval, $synonym_count) = each(%hash_synonym) ) {
#	print "ハッシュ値：$hashval シノニム数：$synonym_count\n";
#}
print "生成されたハッシュの数：$hashval_count\n";
print "シノニムの深さ最大値：$synonym_max\n";


# IPv4アドレスを数値変換する
sub get_ipvalue {
	my $ipaddr = shift;
	
	my @octet = split(/\./, $ipaddr);
	my $ret = 0;
	
	# ビッグエンディアンの場合の計算
	for my $i (0..3) {
		$ret += $octet[$i] * (0x100 ** ($i));
	}
	
	return $ret;
}
