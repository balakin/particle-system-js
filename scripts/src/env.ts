const isProduction = process.env.NODE_ENV === 'production';
const mode = isProduction ? 'Release' : 'Debug';

export const env = {
  isProduction,
  mode,
};
