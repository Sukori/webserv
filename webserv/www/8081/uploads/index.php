<?php
	echo "file saved under the name " . $_FILES['file']['name'];
	file_put_contents($_FILES['file']['name'], file_get_contents($_FILES['file']['tmp_name']));
?>
