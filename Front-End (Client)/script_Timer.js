let timerElement;
let countdownInterval;

function startOrResumeTimer(durationInSeconds) {
  const savedEndTime = localStorage.getItem('countdownEnd');
  const endTime = savedEndTime ? parseInt(savedEndTime, 10) : (Date.now() + durationInSeconds * 1000);
  localStorage.setItem('countdownEnd', endTime);


  // Warn-Markierungen persistent im localStorage speichern
  const warnedAlready = JSON.parse(localStorage.getItem('warnedTimes')) || [];

  countdownInterval = setInterval(() => {
    const remaining = Math.max(0, Math.floor((endTime - Date.now()) / 1000));
    const mins = String(Math.floor(remaining / 60)).padStart(2, '0');
    const secs = String(remaining % 60).padStart(2, '0');

    if (timerElement) {
      timerElement.textContent = `⏱️ ${mins}:${secs}`;
    }

    const warnThresholds = [600, 300, 120];
    warnThresholds.forEach(threshold => {
      if (remaining === threshold && !warnedAlready.includes(threshold)) {
        alert(`Nur noch ${threshold / 60} Minuten übrig!`);
        warnedAlready.push(threshold);
        localStorage.setItem('warnedTimes', JSON.stringify(warnedAlready));
      }
    });

    if (remaining <= 0) {
      clearInterval(countdownInterval);
      alert("Zeit abgelaufen!");
      localStorage.removeItem('countdownEnd');
      localStorage.removeItem('warnedTimes');
    }
  }, 1000);
}

document.addEventListener('DOMContentLoaded', () => {
  timerElement = document.getElementById('globalTimer');
});

// Funktion global verfügbar machen
window.startOrResumeTimer = startOrResumeTimer;