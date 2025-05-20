// Warten bis DOM vollständig geladen ist
window.addEventListener("DOMContentLoaded", () => {
  startOrResumeTimer(); // 20 Minuten

  const inputs = document.querySelectorAll('.number-inputs input');
  const rfidTextElement = document.getElementById("rfidText");
  const checkBtn = document.getElementById("check-btn");

  // Globale Status-Variablen
  window.rfidChecked = false;
  window.idChecked = false;

  const correctRFID = "04824D1FAF6180";
  const correctID = "46108";

  // Eingabelogik für ID-Felder
  inputs.forEach((input, index) => {
    input.addEventListener('input', () => {
      if (input.value.length === 1 && index < inputs.length - 1) {
        inputs[index + 1].focus();
      }
    });

    input.addEventListener('keydown', (e) => {
      if (e.key === "Backspace" && input.value === '' && index > 0) {
        inputs[index - 1].focus();
      }
    });
  });

  // Button-Logik für ID-Check
  checkBtn?.addEventListener('click', () => {
    if (window.idChecked) return;

    let enteredID = '';
    inputs.forEach(input => enteredID += input.value.trim());

    if (enteredID === correctID) {
      window.idChecked = true;
      console.log("✅ Richtige ID eingegeben");

      const instruction = document.getElementById('instruction');
      instruction?.classList.remove('hidden');
    }
 else {
      alert("❌ Falsche ID! Bitte nochmal prüfen.");
    }
  });

  // RFID zyklisch abfragen
  setInterval(() => {
    fetch("http://192.168.4.1/uid")
      .then(res => res.text())
      .then(text => {
        if (rfidTextElement) {
          rfidTextElement.innerText = text;
        }
        console.log("[RFID UID]", text);
        if (text.trim() === correctRFID && !window.rfidChecked) {
          window.rfidChecked = true;
          console.log("✅ Richtige RFID erkannt");
          prüfeBeideUndHandle();
        }
      })
      .catch(err => console.error("[RFID] Fehler", err));
  }, 2000);
});

function prüfeBeideUndHandle(callback = () => {}) {
  const interval = setInterval(() => {
    if (window.rfidChecked && window.idChecked) {
      clearInterval(interval); // Aktion nur einmal ausführen
      console.log("🎯 Beide Checks erfolgreich – starte Aktionen...");

      sende("192.168.4.1", "/sound");
      sende("192.168.4.2", "/green-on");

      setTimeout(() => {
        sende("192.168.4.1", "/sound");
        sende("192.168.4.2", "/green-off");
        callback();

        window.location.href = "index3.html"
      }, 1000);
    }
  }, 1000); // Überprüfe jede Sekunde
}


// Einfacher HTTP-Befehl
function sende(ip, path) {
  fetch(`http://${ip}${path}`)
    .then(res => res.text())
    .then(text => console.log(`[${ip}${path}] OK`, text))
    .catch(err => console.error(`[${ip}${path}] FEHLER`, err));
}
