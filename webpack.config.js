const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyPlugin = require('copy-webpack-plugin');

const production = process.env.NODE_ENV === 'production';

module.exports = {
  mode: production ? 'production' : 'development',
  entry: './src/index.js',
  output: {
    clean: true,
    path: path.resolve(__dirname, 'dist'),
    filename: 'static/js/[name].[contenthash].js',
    chunkFilename: 'static/js/[name].[contenthash].chunk.js',
    assetModuleFilename: 'static/assets/[name].[contenthash][ext]',
  },
  plugins: [
    new CopyPlugin({
      patterns: [
        {
          from: '**/*',
          to: '.',
          context: 'public/',
          noErrorOnMissing: true,
          globOptions: {
            ignore: ['index.html'].map((pattern) => {
              return path.posix.join(path.resolve(__dirname, 'public').replace(/\\/g, '/'), pattern);
            }),
          },
        },
      ],
    }),
    new HtmlWebpackPlugin({
      template: 'public/index.html',
      minify: production
        ? {
            html5: true,
            collapseWhitespace: true,
            minifyCSS: true,
            minifyJS: true,
            minifyURLs: false,
            removeAttributeQuotes: true,
            removeComments: true,
            removeEmptyAttributes: true,
            removeOptionalTags: true,
            removeRedundantAttributes: true,
            removeScriptTypeAttributes: true,
            removeStyleLinkTypeAttributese: true,
            useShortDoctype: true,
          }
        : false,
    }),
  ],
  devServer: {
    static: {
      directory: path.join(__dirname, 'public'),
    },
    open: true,
    compress: true,
    port: 9000,
  },
  performance: production
    ? {
        maxAssetSize: 2097152,
      }
    : {
        hints: false,
      },
};
