% Create the metadata
metadata = [];
metadata = daff_metadata_addKey(metadata, 'Keyname', 'String', 'Tralala');
metadata = daff_metadata_addKey(metadata, 'Lufttemperatur', 'Float', 2.34238);
metadata = daff_metadata_addKey(metadata, 'Zahl', 'Int', 0);
metadata = daff_metadata_addKey(metadata, 'Boolesch', 'Bool', false);

fid = fopen('metadata.dat', 'wb', 'l');
daff_write_metadata(fid, metadata);
fclose(fid);
