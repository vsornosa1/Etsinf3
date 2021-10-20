const fs = require('fs');
let fich = process.argv[2];

function sort_lines(myString) {
    return myString.toString().split("\n").sort().join("\n");
}

console.log('');

var cont = process.argv.length - 2;
for (let i = 2; i <= process.argv.length - 1; i++) {
    fs.readFile(process.argv[i], 'utf8', function (err, data) {
        fs.writeFile(process.argv[i] + "2", sort_lines(data), 'utf8', function () {
            console.log('* Ordenando ' + process.argv[i]);
            if (--cont == 0)
                console.log('-> Ya se han ordenado todos los .txt\n');
        });
    });
}