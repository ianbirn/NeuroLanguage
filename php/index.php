<!DOCTYPE html>
<html>

<head> 
	<title>Language Recognition</title> 
    <link rel="stylesheet" type="text/css" href="style.css">
    <link href="https://fonts.googleapis.com/css?family=Amatic+SC|Raleway:100,200,600,700" rel="stylesheet">
    <meta name="viewport" content ="width=device-width, initial-scale=1.0">
</head> 

<body>
    <img src="logo.jpg">
    <div class="content">
    <h1>Want to know the language?</h1>
    <div class="info">
        <form action="" method="post">
            <textarea class="textbox" name="input" wrap="soft" maxlength="10000"></textarea>
            <input id="button" type=submit name="Detect" value="Detect">
        </form>
    </div>
    <br><br>
    <div id="result">
    <?php
        $D = 10000;
        if(isset($_POST['Detect'])){
            $phrase = $_POST['input'];
            $lang = exec("./langRec $D '$phrase'");
            print($lang);
            $styleBlock = sprintf('
            <style type="text/css">
                #result {
                background-color:%s
                }
            </style>
            ', lightblue);
            echo $styleBlock;
        }
        
    ?> 
    </div>
    <br><br><br>
    <h3>How does it work?</h3>
    <p>1. Type your text into the box above</p>
    <p>2. Press detect and the computer will make its prediction!</p>
    </div>
    <p style="text-align: center;">This is an open source project created by students at Villanova University. <a href="https://github.com/ianbirn/NeuroLanguage" target="_blank">Learn more about the project/authors</a></p>
    <div class="line">
    </div>
</body>
</html>
