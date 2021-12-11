<?php 
$a = new PhpLlrp("169.254.1.1");
echo $a->getHost()."\n";
//$inv = $a->getInvent(5);
//var_dump($inv);
var_dump($a->writeGpo(1,1));
var_dump($a->getGpoState(1));
sleep(2);
var_dump($a->writeGpo(1,0));
var_dump($a->getGpoState(1));

while (1) {
    var_dump($a->getGpiState(1));
    sleep(3);
}