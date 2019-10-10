// @flow
import * as React from 'react';
import { type StorageProvider, type FileMetadata } from '../index';
import DownloadSaveAsDialog from './DownloadSaveAsDialog';
import SaveAlt from '@material-ui/icons/SaveAlt';

/**
 * "Storage" allowing to download a copy of the game.
 * Used for the web-app.
 */
export default ({
  name: 'Download a copy', //TODO: i18n
  renderIcon: () => <SaveAlt />,
  hiddenInOpenDialog: true,
  createOperations: ({ setDialog, closeDialog }) => ({
    onSaveProjectAs: (project: gdProject, fileMetadata: ?FileMetadata) => {
      return new Promise(resolve => {
        setDialog(() => (
          <DownloadSaveAsDialog
            onDone={() => {
              closeDialog();
              resolve({ wasSaved: false, fileMetadata });
            }}
            project={project}
          />
        ));
      });
    },
  }),
}: StorageProvider);
