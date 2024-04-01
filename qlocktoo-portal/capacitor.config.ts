import { CapacitorConfig } from '@capacitor/cli';

const config: CapacitorConfig = {
  appId: 'nl.qlocktoo',
  appName: 'QlockToo',
  webDir: 'dist/qlocktoo-portal',
  server: {
    hostname: '127.0.0.1',
    cleartext: true,
    allowNavigation: ['*'],
  }
};

export default config;
