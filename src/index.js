import { init } from './particle-system';

(async () => {
  await init({
    canvas: document.getElementById('canvas'),
  });
})();
