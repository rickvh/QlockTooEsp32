import { CapacitorConfig } from '@capacitor/cli';

const config: CapacitorConfig = {
  appId: 'nl.qlocktoo',
  appName: 'QlockToo',
  webDir: 'dist',
  server: {
    androidScheme: 'https'
  }
};

export default config;
