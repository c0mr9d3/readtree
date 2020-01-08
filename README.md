<p>This program recursive print files in directory.

<h3>Makefile:</h3>
<code>make</code> - default: compile and run<br>
<code>make run</code> - run program<br>
<code>make compile</code> - compile program<br><br>

Also you can compile:<br>
<code>gcc readtree.c -o readtree</code><br>

And run:<br>
<code>./readtree [dir]</code><br>

Example:<br>
<code>./readtree .</code>
<h3>Help:</h3>
       <code>./readtree</code> [options] [dir]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Options:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-h help<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-s <size> ( > 0) size of file<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-f exclude FIFO<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-d exclude directory<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-b exclude block device<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-l exclude symlink<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-r exclude regular file<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-c exclude character device<br>
</p>
