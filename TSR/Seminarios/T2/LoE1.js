const fs = require('fs');
let fich = process.argv[2];

function sort_lines(myString) {
    return myString.toString().split("\n").sort().join("\n");
}

var lecturaFich = fs.readFileSync(fich, 'utf8');
fs.writeFileSync(fich + "2", sort_lines(lecturaFich), 'utf8');
