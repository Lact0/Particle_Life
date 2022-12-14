//Vars
let width = window.innerWidth;
let height = window.innerHeight;
let canvas;
let ctx;
const colors = ['red', 'green', 'blue'];

//Useful Functions
function max(n1, n2) {
  if(n1 > n2) {
    return n1;
  }
  return n2;
}

function min(n1, n2) {
  if(n1 < n2) {
    return n1;
  }
  return n2;
}

function randColor() {
  return 'rgba(' + rand(0,255) + ',' + rand(0,255) + ',' + rand(0,255) + ')';
}

function rand(min, max) {
  return Math.floor(Math.random() * (max-min+1)) + (min);
}
function degToRad(degrees) {
  return degrees * Math.PI / 180;
}

function radToDeg(rad) {
  return rad / Math.PI * 180;
}

function drawLine(x1, y1, x2, y2, style = white, r = 1) {
  ctx.strokeStyle = style;
  ctx.lineWidth = r;
  ctx.beginPath();
  ctx.moveTo(x1, y1);
  ctx.lineTo(x2, y2);
  ctx.stroke();
}

function equals(arr1, arr2) {
  if(arr1.length != arr2.length) {
    return false;
  }
  for(let i = 0; i < arr1.length; i++) {
    if(arr1[i] != arr2[i]) {
      return false;
    }
  }
  return true;
}

function copy(arr) {
  return JSON.parse(JSON.stringify(arr));
}

function remove(arr, n) {
  const i = arr.indexOf(n);
  if(i >= 0) {
    arr.splice(i, 1);
    return true;
  }
  return false;
}

function shuffle(arr) {
  let m = arr.length - 1;
  while(m > 0) {
    const i = rand(0, m);
    const temp = arr[i];
    arr[i] = arr[m];
    arr[m] = temp;
    m--;
  }
  return arr;
}

function intersects(p11, p12, p21, p22) {
  const m1 = (p11.y - p12.y) / (p11.x - p12.x);
  const m2 = (p21.y - p22.y) / (p21.x - p22.x);
  const x = ((m1 * p11.x) - (m2 * p21.x) - p11.y + p21.y) / (m1 - m2);
  if((x > p11.x != x > p12.x) && (x > p21.x != p22.x)) {
    return {'x': x, 'y': m1 * (x - p11.x) + p11.y};
  }
  return false;
}

//Classes
class Vector {
  constructor(x = 0, y = 0, x0 = 0, y0 = 0) {
    this.x = x - x0;
    this.y = y - y0;
    this.getMag();
  }

  getMag() {
    this.mag = Math.sqrt(Math.pow(this.x, 2) + Math.pow(this.y, 2));
  }

  normalize() {
    if(this.mag == 0) {
      return;
    }
    this.x /= this.mag;
    this.y /= this.mag;
    this.getMag();
  }

  setMag(mag) {
    this.normalize();
    this.x *= mag;
    this.y *= mag;
    this.mag = mag;
  }

  limit(mag) {
    this.getMag();
    if(this.mag > mag) {
      this.setMag(mag);
    }
  }

  copy() {
    return new Vector(this.x, this.y);
  }

  add(vector) {
    this.x += vector.x;
    this.y += vector.y;
    this.getMag();
  }

  sub(vector) {
    this.x -= vector.x;
    this.y -= vector.y;
    this.getMag();
  }

  distTo(vector) {
    return Math.sqrt(Math.pow(this.x - vector.x, 2) + Math.pow(this.y - vector.y, 2));
  }
}

class Particle {
  constructor(params = {}) {
    const x = params.x || rand(0, width - 1);
    const y = params.y || rand(0, height - 1);
    this.pos = new Vector(x, y);
    this.r = params.r || 5;
    this.type = params.type || rand(0, colors.length - 1);
    this.vel = new Vector();
  }

  draw() {
    ctx.strokeStyle = colors[this.type];
    ctx.beginPath();
    ctx.arc(this.pos.x, this.pos.y, this.r, 0, Math.PI * 2);
    ctx.stroke();
  }
}

class Box {
  constructor(x, y, w, h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
  }

  draw() {
    ctx.strokeStyle = 'white';
    ctx.strokeRect(this.x, this.y, this.w, this.h);
  }

  contains(point) {
    return point.x == min(max(this.x, point.x), this.x + this.w) && point.y == min(max(this.y, point.y), this.y + this.h);
  }

  overlaps(box) {
    return (max(box.x, this.x) <= min(box.x + box.w, this.x + this.w)) && (max(box.y, this.y) <= min(box.y + box.h, this.y + this.h));
  }
}

class Quadtree {
  constructor(x, y, w, h) {
    this.box = new Box(x, y, w, h);
    this.store = [];
    this.leaf = true;
    this.children = [];
  }

  pass(obj) {
    if(!this.box.contains(obj.pos)) {
      return false;
    }
    if(this.leaf) {
      this.store.push(obj);
      if(this.store.length > 1) {
        this.split();
      }
    } else {
      this.passToLeaves(obj);
    }
    return true;
  }

  passToLeaves(obj) {
    for(let tree of this.children) {
      tree.pass(obj);
    }
  }

  split() {
    this.leaf = false;
    this.children.push(new Quadtree(this.box.x, this.box.y, this.box.w / 2, this.box.h / 2));
    this.children.push(new Quadtree(this.box.x + this.box.w / 2, this.box.y, this.box.w / 2, this.box.h / 2));
    this.children.push(new Quadtree(this.box.x, this.box.y + this.box.h / 2, this.box.w / 2, this.box.h / 2));
    this.children.push(new Quadtree(this.box.x + this.box.w / 2, this.box.y + this.box.h / 2, this.box.w / 2, this.box.h / 2));
    for(let obj of this.store) {
      this.passToLeaves(obj);
    }
    this.store = [];
  }

  draw() {
    this.box.draw();
    for(let tree of this.children) {
      tree.draw();
    }
  }
}
