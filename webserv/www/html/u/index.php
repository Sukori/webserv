<?php
var_dump($GLOBALS);
var_dump(file_get_contents($_FILES['file']['tmp_name']));
//file_put_contents($_FILES['file']['name'], file_get_contents($_FILES['file']['tmp_name']));
?>
