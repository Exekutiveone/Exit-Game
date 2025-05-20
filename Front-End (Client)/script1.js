window.addEventListener('DOMContentLoaded', () => {
  startOrResumeTimer(1200); // 20 Minuten

  const inputs = document.querySelectorAll('.number-inputs input');
  const instructionBox = document.getElementById('instruction');
  const errorBox = document.getElementById('error-message');
  const callSound = document.getElementById('callSound');
  const nextPageBtn = document.getElementById('nextPageBtn');

  // Anfangszustand des "Weiter"-Buttons
  nextPageBtn.style.display = 'none';

  function callHotline() {
    errorBox.classList.add('hidden');
    instructionBox.classList.add('hidden');

    let enteredCode = '';
    inputs.forEach(input => enteredCode += input.value);

    const correctCode = "0472581963";

    if (enteredCode === correctCode) {
      instructionBox.classList.remove('hidden');
      callSound.play();

      // Nach genau 10 Sekunden den "Weiter"-Button anzeigen
      setTimeout(() => {
        nextPageBtn.style.display = 'block';
      }, 15000);

    } else {
      errorBox.classList.remove('hidden');
      setTimeout(() => {
        errorBox.classList.add('hidden');
      }, 10000);
    }
  }

  document.querySelector('.call-button').addEventListener('click', callHotline);

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
});