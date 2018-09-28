#!/bin/sh

# cd to "build" directory before running this script

#echo "checkKBBs..."
./opencog/das/IntegrationTests/checkKBBs ../opencog/das/IntegrationTests/data/QueryToyExample.scm > /tmp/checkKBBs.output
diff /tmp/checkKBBs.output ../opencog/das/IntegrationTests/baselines/checkKBBs.txt

#echo "dasCreation..."
./opencog/das/IntegrationTests/dasCreation ../opencog/das/IntegrationTests/data/QueryToyExample.scm > /tmp/dasCreation.output
diff /tmp/dasCreation.output ../opencog/das/IntegrationTests/baselines/checkKBBs.txt

#echo "queryExample..."
./opencog/das/IntegrationTests/queryExample ../opencog/das/IntegrationTests/data/QueryToyExample.scm > /tmp/queryExample.output
diff /tmp/queryExample.output ../opencog/das/IntegrationTests/baselines/queryExample.txt
