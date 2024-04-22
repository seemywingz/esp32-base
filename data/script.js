
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
}

window.onload = function () {
    loadConfig();
};

async function saveConfig() {
    try {
        const response = await fetch('/config', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(config), // Use the global config object directly
        });

        if (response.ok) {
            alert('Configuration saved successfully!');
        } else {
            throw new Error('Failed to save configuration');
        }
    } catch (error) {
        console.error('Error saving configuration:', error);
        alert('Error saving configuration');
    }
}

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

    switch (setting) {
        case 'wifi':
            wifiSettings.style.display = 'block';
            saveSettings.style.display = 'block';
            break;
        case 'ap':
            apSettings.style.display = 'block';
            saveSettings.style.display = 'block';
            break;
        case 'mode':
            modeSettings.style.display = 'block';
            break;
        case 'about':
            aboutSettings.style.display = 'block';
            break;
    }
});

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
