### introduction

This is beta, dont expect too much, maybe later will be more...

### instalation 

First you need install php-cpp 

```bash
git clone https://github.com/CopernicaMarketingSoftware/PHP-CPP.git
cd PHP-CPP
make
sudo make install
```

Go to php-llrp-ext dir and open Makefile, you need to change php paths 

```
LIBRARY_DIR		= /usr/lib/php/20190902/
PHP_CONFIG_DIR	= /etc/php/7.4/cli/conf.d/
```

Maybe you need to install libcrypto++-dev, libssl-dev

Then:

```bash
make
sudo make install
```


PHP example: 

```php
$a = new PhpLlrp("169.254.1.1");
echo $a->getHost()."\n";
$inv = $a->getInvent(5); //5 sec to listen tags
var_dump($inv);

var_dump($a->writeGpo(1,1)); // $portNumber, $state
var_dump($a->getGpoState(1)); // $portNumber
sleep(2);
var_dump($a->writeGpo(1,0));
var_dump($a->getGpoState(1));

var_dump($a->getGpiState(1)); // $portNumber
```




