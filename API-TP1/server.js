const express = require('express');
const cors = require('cors');
const rateLimit = require('express-rate-limit');
const bodyParser = require('body-parser');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');
const path = require('path');
const cookieParser = require('cookie-parser');

// Récupération des paramètres de la base de données
const connection = require('./db');
const { ok } = require('assert');

// App principale
const app = express();

// Paramètres servant au fonctionnement du serveur et de la sécurité de celui-ci
const port = 3000;
const jwtKey = "Key";
const Limiter = rateLimit({
    windowMs: 15 * 60 * 1000, // 15 minutes
    max: 500, // Limite chaque IP à 500 requêtes par fenêtre
    message: 'Trop de requêtes effectuées depuis cette IP, veuillez réessayer plus tard.',
});

// Utilisation des paramètres définis précédemment
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(cookieParser());
app.use(Limiter);

// Fonction pour vérifier les inputs pour éviter les injections SQL ou XSS
function containsInvalidChars(input) {
    const forbiddenChars = /['";<>]/; // Liste des caractères interdits
    return forbiddenChars.test(input); // Renvoie true si un des caractères est trouvé
}

// Route d'inscription via l'API
app.post('/register', (req, res) => {
    const { mail, name, password } = req.body;
    if (!mail || !name || !password) return res.status(400).json({ message: "Le nom d'utilisateur ou le mot de passe est manquant !" });

    let errorMessage = '';

    // Vérification des champs individuellement
    if (containsInvalidChars(name)) {
        errorMessage += 'Le nom contient des caractères interdits. ';
    }

    if (containsInvalidChars(mail)) {
        errorMessage += 'L\'email contient des caractères interdits. ';
    }

    if (containsInvalidChars(password)) {
        errorMessage += 'Le mot de passe contient des caractères interdits. ';
    }

    // Si des erreurs existent, afficher le message d'erreur
    if (errorMessage !== '') {
        return res.status(400).json({ message: errorMessage });
    }

    const sqlVerif = `SELECT mail FROM User WHERE mail = ?`;

    connection.query(sqlVerif, [mail], (err, result) => {
        if (err) {
            console.error("Erreur lors de la vérification d'inscription\nErreur SQL :\n", err);
            return res.status(500).json({ message: "Erreur lors de la création du compte" });
        }

        if (result.length > 0) {
            return res.status(401).json({ message: "Mail déjà utilisé" });
        }
        bcrypt.genSalt(10, (err, salt) => {
            if (err) {
                console.error("Erreur lors de la génération du sel Bcrypt\nErreur Bcrpt Salt:\n", err);
                return res.status(500).json({ message: "Erreur lors de la création du compte" });
            }

            bcrypt.hash(password, salt, (hashErr, hashedPassword) => {
                if (hashErr) {
                    console.error('Erreur lors du hachage du mot de passe\nErreur Bcrypt Hash :\n', hashErr);
                    return res.status(500).json({ message: "Erreur lors de la création du compte" });
                }

                let token = jwt.sign({ mail }, jwtKey);

                const sqlNewUser = `INSERT INTO User (mail, username, password, token) VALUES (?, ?, ?, ?)`;

                connection.query(sqlNewUser, [mail, name, hashedPassword, token], (sqlErr) => {
                    if (sqlErr) {
                        console.error("Erreur lors de la création du nouvel User\nErreur sql :\n", sqlErr);
                        return res.status(500).json({ message: "Erreur lors de la création du compte" });
                    }

                    res.cookie('userToken', token, {
                        httpOnly: false,
                        secure: false,
                        maxAge: 43200000
                    });

                    return res.status(200).json({ message: "Compte créé avec succès" });
                });
            });
        });
    });
});

// Route de connexion via l'API
app.post('/login', (req, res) => {
    const { mail, password } = req.body;

    let errorMessage = '';

    if (containsInvalidChars(mail)) {
        errorMessage += 'L\'email contient des caractères interdits. ';
    }

    if (containsInvalidChars(password)) {
        errorMessage += 'Le mot de passe contient des caractères interdits. ';
    }

    // Si des erreurs existent, afficher le message d'erreur
    if (errorMessage !== '') {
        return res.status(400).json({ message: errorMessage });
    }

    if (!mail || !password) return res.status(400).json({ message: "Nom ou mot de passe manquant !" });

    const sqlLogin = 'SELECT password, token FROM User WHERE mail = ?';

    connection.query(sqlLogin, [mail], (err, result) => {
        if (err) {
            console.error("Erreur lors de la requête SQL login\nErreur sql :\n", err);
            return res.status(500).json({ message: "Erreur lors de la connexion" });
        }

        if (result.length === 0) {
            return res.status(401).json({ message: "Mail inconnu" });
        }

        const isPasswordValid = bcrypt.compareSync(password, result[0].password);

        if (isPasswordValid) {
            const sqlLoginToken = 'UPDATE User SET token = ? WHERE mail = ?';
            let token = jwt.sign({ mail }, jwtKey);

            connection.query(sqlLoginToken, [token, mail], (err) => {
                if (err) {
                    console.error("Erreur lors de la requête LoginToken\nErreur sql :\n", err);
                    return res.status(500).json({ message: "Erreur lors de la création du token" });
                }

                res.cookie('userToken', token, { 
                    httpOnly: false,
                    secure: false,
                    maxAge: 43200000
                });
                return res.status(200);
            });
        } else {
            return res.status(401).json({ message: "Mot de passe incorrect" });
        }
    });
});

// Route de déconnexion via l'API
app.get('/disconnect', (req, res) => {
    const token = req.cookies.userToken;

    if (containsInvalidChars(token)) {
        return res.status(400).json({ message: 'le token contient des caractères invalides' });
    }

    if (!token) return res.status(400).json({ message: "Le token du User est requis pour la déconnexion" });

    const sqlDisconnect = 'UPDATE User SET token = NULL WHERE token = ?';

    connection.query(sqlDisconnect, [token], (err) => {
        if (err) {
            console.error("Erreur lors de la requête de déconnexion\n Erreur sql:\n", err);
            return res.status(500).json({ message: "Erreur lors de la déconnexion" });
        }
        res.clearCookie('userToken');
        return res.status(200).json({ message: "Utilisateur déconnecté" });
    });
});

const authenticate = (req, res, next) => {
    const token = req.cookies.userToken;

    if (!token) {
        return res.redirect('/');
    }

    jwt.verify(token, jwtKey, (err, decoded) => {
        if (err) {
            return res.redirect('/');
        }

        req.user = decoded;
        next();
    });
};

app.get('/index.html', authenticate, (req, res) => {
    res.sendFile(path.join(__dirname, '../index.html'));
});

// Route par défaut pour envoyer la page de connexion
app.get('/', (req, res) => {
    if(req.cookies.userToken) return res.redirect('/index.html');
    res.sendFile(path.join(__dirname, '../login.html'));
});

// Lancement du serveur sur un port défini
app.listen(port, () => {
    console.log(`Le serveur NodeJS est en écoute sur le port ${port}`);
});