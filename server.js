const mysql = require('mysql');
const express = require('express');
const app = express();
const port = 3000;

const connection = mysql.createConnection({
    host: '192.168.64.184',
    user: 'site',
    password: 'site',
    database: 'TP2-CIEL2'
});

connection.connect(err => {
    if (err) {
        console.error('Erreur de connexion à la base de données:', err);
        return;
    }
    console.log('Connected to the database!');
});

app.use(express.static('/var/www/html/CIEL2-TP2'));

app.get('/', (req, res) => {
    res.sendFile('/var/www/html/CIEL2-TP2/index.html');
});

app.get('/api/gps', (req, res) => {
    connection.query('SELECT latitude, longitude FROM gps ORDER BY id DESC LIMIT 1', (err, results) => {
        if (err) {
            console.error('Erreur SQL :', err);
            return res.status(500).send('Erreur lors de la récupération des données GPS.');
        }

        console.log('Résultat SQL :', results);

        if (!results || results.length === 0) {
            return res.status(404).send('Aucune donnée GPS trouvée.');
        }

        const { latitude, longitude } = results[0];
        res.json({ latitude, longitude });
    });
});

app.listen(port, () => {
    console.log(`Server running on http://192.168.64.184:${port}`);
});
