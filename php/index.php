<!DOCTYPE html>
<html>

<head> 
	<title>Language Recognition</title> 
    <link rel="stylesheet" type="text/css" href="style.css">
    <link href="https://fonts.googleapis.com/css?family=Amatic+SC|Raleway:100,200,600,700" rel="stylesheet">
</head> 

<body>
    <img src="NUEROLANGUAGE.jpg">

    <br>
    <form action="" method="post">
        Want to know a language?
        <textarea class="textbox" name="input" rows="1" cols="50" wrap="soft" maxlength="10000"></textarea>
        <input class="button" type=submit name="Detect" value="Detect">

    </form>

    <br>
    <h4>Functions:</h4>
    <input id="low" class="button" type=submit name="Low" value="Low">
    <input id="medium" class="button" type=submit name="Medium" value="Medium">
    <input id="high" class="button" type=submit name="High" value="High">

    <br><br>

    <?php
        $D = 10000;
        if(isset($_POST['Low'])){
            $D = 3000;
        } else if(isset($_POST['Medium'])){
            $D = 5000;
        } else if(isset($_POST['High'])){
            $D = 10000;
        }

        if(isset($_POST['Detect'])){
            $phrase = $_POST['input'];
            $lang = exec("./langRec $D '$phrase'");
            print($lang);
        }
        
    ?> 

</body>
</html>
