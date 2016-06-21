var addon = require('./build/Release/addon');
var calculations = process.argv[2] || 100000000;

var runs = 0;

function runAsync() {

  function done (err, result) {
      console.log(runs, err, result, process.memoryUsage());
      if(runs++ < 50000){
        runAsync();
      } else {
        console.log("YEAAAAAH");
      }
  }

  addon.calculateAsync(Buffer([0x00, 0x8, 0x0, 0x0], "binary"), done);

}

runAsync();

function foo(){
  console.log(arguments);
}

process.on('uncaughtException', foo);
process.on('exit', foo);
process.on('beforeExit', foo);