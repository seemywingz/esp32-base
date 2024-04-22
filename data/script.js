
let config = {};

async function loadConfig() {
    try {
        const response = await fetch('/config');
        if (response.ok) {
            config = await response.json();
            updateUI();
        } else {
            throw new Error('Failed to load configuration');
        }
    } catch (error) {
        console.error('Error loading configuration:', error);
        // alert('Error loading configuration');
    }
}

function updateUI() {
    document.getElementById('wifiSSID').value = config.client.ssid || '';
    document.getElementById('wifiPassword').value = config.client.password || '';
    document.getElementById('apSSID').value = config.ap.ssid || '';
    document.getElementById('apPassword').value = config.ap.password || '';
    document.getElementById('currentMode').innerText = config.mode || '';
}

window.onload = function () {
    loadConfig();
};

var settingSelect = document.getElementById('settingSelect');
settingSelect.addEventListener('change', function () {
    var setting = settingSelect.value;

    var wifiSettings = document.getElementById('wifi-settings');
    wifiSettings.style.display = 'none';

    var aboutSettings = document.getElementById('about-settings');
    aboutSettings.style.display = 'none';

    var apSettings = document.getElementById('ap-settings');
    apSettings.style.display = 'none';

    var saveSettings = document.getElementById('save-settings');
    saveSettings.style.display = 'none';

    var modeSettings = document.getElementById('mode-settings');
    modeSettings.style.display = 'none';

    var apModeSettings = document.getElementById('ap-mode-settings');
    apModeSettings.style.display = 'none';

    var clientModeSettings = document.getElementById('client-mode-settings');
    clientModeSettings.style.display = 'none';

    switch (setting) {
        case 'wifi':
            wifiSettings.style.display = 'block';
            saveSettings.style.display = 'block';
            clientModeSettings.style.display = 'block';
            break;
        case 'ap':
            apSettings.style.display = 'block';
            saveSettings.style.display = 'block';
            apModeSettings.style.display = 'block';
            break;
        case 'mode':
            modeSettings.style.display = 'block';
            apModeSettings.style.display = 'block';
            clientModeSettings.style.display = 'block';
            break;
        case 'about':
            aboutSettings.style.display = 'block';
            break;
    }
});

function saveConfig() {
    fetch('/config', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(config),
    })
        .then(response => {
            if (response.ok) {
                alert('Configuration saved successfully!');
                return response.json(); // Assuming the server responds with JSON
            } else {
                throw new Error('Failed to save configuration');
            }
        })
        .then(data => {
            console.log('Save successful:', data);
        })
        .catch(error => {
            console.error('Error saving configuration:', error);
            alert('Error saving configuration');
        });
}

async function restart() {
    try {
        const response = await fetch('/restart', {
            method: 'POST',
        });

        if (response.ok) {
            alert('Restarting in ' + JSON.stringify(config.mode) + 'mode');
        } else {
            throw new Error('Failed to restart');
        }
    } catch (error) {
        console.error('Error restart:', error);
        alert('Error restart');
    }
}

document.getElementById('saveButton').addEventListener('click', function () {
    config.client.ssid = document.getElementById('wifiSSID').value;
    config.client.password = document.getElementById('wifiPassword').value;
    config.ap.ssid = document.getElementById('apSSID').value;
    config.ap.password = document.getElementById('apPassword').value;
    saveConfig();
});

document.getElementById('apModeButton').addEventListener('click', function () {
    config.mode = 'ap';
    saveConfig();
    restart();
});

document.getElementById('clientModeButton').addEventListener('click', function () {
    config.mode = 'client';
    saveConfig();
    restart();
});
