import { CapacitorConfig } from '@capacitor/cli';

const config: CapacitorConfig = {
  appId: 'nl.qlocktoo',
  appName: 'QlockToo',
  webDir: 'dist/qlocktoo-portal',
  server: {
    androidScheme: 'https'
  }
};

export default config;
