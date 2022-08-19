window.onresize = changeWindow;
const balls = [];
let rules = [];
let maxForce = 1;

function load() {
  canvas = document.querySelector('.canvas');
  ctx = canvas.getContext('2d');
  canvas.width = width;
  canvas.height = height;
  document.onkeydown = keyPress;
  for(let i = 0; i < 100; i++) {
    balls.push(new Particle());
  }
  for(let i = 0; i < colors.length; i++) {
    const rule = [];
    for(let j = 0; j < colors.length; j++) {
      rule.push(Math.random() * 2 - 1);
    }
    rules.push(rule);
  }
  runFrame();
}

function runFrame() {
  ctx.clearRect(0, 0, width, height);
  for(let i = 0; i < balls.length; i++) {
    const ball = balls[i];
    for(let j = 0; j < balls.length; j++) {
      if(j == i) {
        continue;
      }
      const otherBall = balls[j];
      const dist = ball.pos.distTo(otherBall.pos);
      const rule = rules[otherBall.type][ball.type];
    }
  }
  //DO ALL DRAWING HERE
  for(let ball of balls) {
    ball.draw();
  }
  requestAnimationFrame(runFrame);
}

function changeWindow() {
  width = window.innerWidth;
  height = window.innerHeight;
  //REDRAW SCREEN
}

function keyPress(key) {
  if(key.keyCode != 32) {
    return;
  }
}

function leftClick() {
  const x = event.clientX;
  const y = event.clientY;
}
