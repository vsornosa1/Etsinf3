const fs = require('fs');
let fich = process.argv[2];

var lecturaFich = fs.readFileSync(fich, function (err, data) {
    if (!err) {
        fs.writeFile(fich, function (err) {
            console.log('Finalizado')
        });
    }
});
