<html>
<head>
<title>GUI</title>
<style type="text/css">
        h1 {color: #75af9e}
        h2 {color: #000000}
        p {color: #000000}
        .header {background-color: #000000}
        .options {background-color: #dddddd; float:left}
        .preview {background-color: #eeeeee}
</style>
</head>

<body>
<div class="header">
        <h1>Custom Packet Generator</h1>
</div>

<div class="options">
        <h2>Options:</h2>

        <form method="POST" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">
        <p>
        Destination IP: <input type="text" name="dip" />
        </p>
        <input type="submit" value="Generate" name="gen" />
        </form>
</div>

<div class="preview">
        <h2>Preview:</h2>
        <p><?php
        $diplocal = $_POST["dip"];
        $hex = `./scapyinterface.py $diplocal`;
        echo $hex;
        echo "<br/>";
        echo strlen($hex);
        echo "<br/>";
        $lines = (int) (strlen($hex)/51);
        echo $lines;
        echo "<br/><br/>";
        for ($l=0; $l<=$lines; $l++){
                echo substr($hex, ($l * 74), 74);
                echo "<br/>";
        }
        ?></p>

        <p>00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00<br/>
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00<br/>
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00<br/>
           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00<br/>
           00 00 00 00 00 00 00 00 00
        </p>
</div>
</body>
</html>
