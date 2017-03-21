<html>
<head>
<title>GUI</title>
<link rel="stylesheet" type="text/css" href="pstyle.css">
</head>
<body>

<div class="header">
<h1>Custom Packet Generator</h1>
</div>



<div class="options">
<h2>Options:</h2>
<form method="POST" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">
        <h3>Destination MAC: <input type="text" name="dmac" /></h3>
        <h3>Source MAC: <input type="text" name="smac" /></h3>
        <h3>IP type: <input type="text" name="ipt" /></h3>
        <h3>Destination IP: <input type="text" name="dip" /></h3>
        <input type="submit" value="Generate" name="gen" />
</form>
</div>



<div class="preview">
<h2>Preview:</h2>
<p><?php
$dmac = $_POST["dmac"];
$smac = $_POST["smac"];
$ipt = $_POST["ipt"];
$dip = $_POST["dip"];
exec("./scapyinterface.py " . $dip, $pvals);

echo "<span id=\"ether\"><span id=\"dstmac\">" . $dmac . "</span> ";
                   echo "<span id=\"srcmac\">" . $smac . "</span> ";
                   echo "<span id=\"type\">"   . $ipt  . "</span></span>";
echo "<span id=\"ip\"><span id=\"hdrlen\">" . $pvals[0] . "</span> ";
                echo "<span id=\"magic\">"  . $pvals[1] . "</span></span><br/>";

echo "<span id=\"ip\"><span id=\"tlen\">"   . $pvals[2] . "</span> ";
                echo "<span id=\"flags\">"  . $pvals[3] . "</span> ";
                echo "<span id=\"frag\">"   . $pvals[4] . "</span> ";
                echo "<span id=\"ttl\">"    . $pvals[5] . "</span> ";
                echo "<span id=\"proto\">"  . $pvals[6] . "</span> ";
                echo "<span id=\"hdrcs\">"  . $pvals[7] . "</span> ";
                echo "<span id=\"srcip\">"  . $pvals[8] . "</span> ";
                echo "<span id=\"dstip1\">" . $pvals[9] . "</span></span><br/>";

echo "<span id=\"ip\"><span id=\"dstip2\">" . $pvals[10]. "</span></span>";
echo "<span id=\"udp\"><span id=\"srcport\">" . $pvals[11] . "</span> ";
                 echo "<span id=\"dstport\">" . $pvals[12] . "</span> ";
                 echo "<span id=\"len\">"     . $pvals[13] . "</span> ";
                 echo "<span id=\"chksum\">"  . $pvals[14] . "</span></span>";
?></p>
</div>

</body>
</html>
