const express = require('express');
const path = require('path');
const mysql = require('mysql');
const app = express();

const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'root',
    database: 'TP2-CIEL2'
});

db.connect((err) => {
    if (err) throw err;
    console.log('Connecte a la base de donnees!');
});

app.use(express.static(path.join('/var/www/html/CIEL2-TP2')));

app.get('/', (req, res) => {
    res.sendFile(path.join('/var/www/html/CIEL2-TP2', 'index.html'));
});

app.get('/api/gps', (req, res) => {
    db.query('SELECT latitude, longitude FROM gps ORDER BY id DESC LIMIT 1', (err, result) => {
        if (err) {
            res.status(500).send('Erreur de recuperation des donnees GPS');
        } else {
            const gpsData = result[0];
            res.json({
                latitude: gpsData.latitude,
                longitude: gpsData.longitude
            });
        }
    });
});

app.listen(3000, () => {
    console.log('Serveur Node.js en ecoute sur http://192.168.64.184:3000');
});
