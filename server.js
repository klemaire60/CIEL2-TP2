const express = require('express');
const mysql = require('mysql2');
const app = express();
const port = 3000;

const connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: 'root',
  database: 'TP2-CIEL2'
});

// Middleware pour servir les fichiers statiques
app.use(express.static('/var/www/html/CIEL2-TP2'));

// Route pour récupérer les trames GPS depuis la base de données
app.get('/api/gps', (req, res) => {
  const query = 'SELECT id, latitude, longitude FROM gps ORDER BY id DESC LIMIT 1';

  connection.query(query, (err, results) => {
    if (err) {
      console.error('Erreur lors de la récupération des données GPS:', err);
      res.status(500).send('Erreur serveur');
      return;
    }

    if (results.length > 0) {
      const gpsData = {
        id: results[0].id,
        latitude: results[0].latitude,
        longitude: results[0].longitude,
      };
      res.json(gpsData);
    } else {
      res.status(404).send('Aucune donnée GPS trouvée');
    }
  });
});

app.listen(port, () => {
  console.log(`Serveur Node.js lancé sur http://192.168.64.184:${port}`);
});
