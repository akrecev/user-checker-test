const express = require('express');
const path = require('path');
const open = require('open');
const addon = require('../build/Release/addon');

const app = express();
const PORT = 3000;

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.json());

app.post('/check', (req, res) => {
    const { username } = req.body;
    const privilege = addon.checkUserPrivilege(username);
    res.send({ username, privilege });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
    open(`http://localhost:${PORT}`);
});
