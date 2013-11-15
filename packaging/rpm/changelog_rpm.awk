BEGIN{
	FS="\n";
	RS="\n\n\n";
}
{
	tmpdate=substr($1, 0, 10)
	gsub(/-/, " ", tmpdate)
	tm=mktime(tmpdate " 00 00 00")
	sub(/^........../, "* " strftime("%a %b %d %Y", tm), $1);
	title=substr($1, 0, 17) substr($1, 33)

	sub(/0\.1\.9\.9/, "0.1.9-1", $3);
	sub(/0\.1\.9:/, "0.1.9-0", $3);

	match($3, /[0-9\.]+-[0-9]+/);
	if (RLENGTH > 0)
	{
		print title " - " substr($3,RSTART, RLENGTH);
	}
	else
	{
		match($3, /[0-9\.]+/);
		print title " - " substr($3,RSTART, RLENGTH) "-1";
	}

	for(i=4; i<=NF; ++i)
	{
		sub(/^\t *[0-9]+\./, "-", $i)
		print $i;
	}
	print ""
}
