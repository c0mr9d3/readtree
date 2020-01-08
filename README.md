<p>This program recursive print files in directory.

<h4>Makefile:</h4>
<code>make</code> - default: compile and run<br>
<code>make run</code> - run program<br>
<code>make compile</code> - compile program<br>

Also you can compile:<br>
<code>gcc readtree.c -o readtree</code><br>
And run:<br>
<code>./readtree</code><br>

<h4>Help:</h4><br>
Usage: readtree [options] <dir><br>
           Options:<br>
           -h help<br>
           -s <size> ( > 0) size of file<br>
           -f exclude FIFO<br>
           -d exclude directory<br>
           -b exclude block device<br>
           -l exclude symlink<br>
           -r exclude regular file<br>
           -c exclude character device<br>
</p>
