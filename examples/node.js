/*

 Will output :

 1
 2
 3
 4

 */

var spawn = require("child_process").spawn;

console.log("1");
spawn( "../phook", [ "-e", "echo 2", "-a", "echo 4" ], { 
	stdio: [process.stdin, process.stdout, process.stderr] 
} );
setTimeout( function() { 
	console.log("3"); 
	process.exit(0);
}, 1000 );
