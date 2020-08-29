package com.tm.kafka.connect.aws.lambda;

import org.apache.kafka.common.config.ConfigDef;
import org.apache.kafka.connect.connector.Task;
import org.apache.kafka.connect.sink.SinkConnector;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class AwsLambdaSinkConnector extends SinkConnector {
  private static Logger log = LoggerFactory.getLogger(AwsLambdaSinkConnector.class);
  private AwsLambdaSinkConnectorConfig config;

  @Override
  public String version() {
    return VersionUtil.getVersion();
  }

  @Override
  public void start(Map<String, String> map) {
    config = new AwsLambdaSinkConnectorConfig(map);
  }

  @Override
  public Class<? extends Task> taskClass() {
    return AwsLambdaSinkTask.class;
  }

  @Override
  public List<Map<String, String>> taskConfigs(int maxTasks) {
    Map<String, String> taskProps = new HashMap<>(config.originalsStrings());
    List<Map<String, String>> taskConfigs = new ArrayList<>(maxTasks);
    for (int i = 0; i < maxTasks; ++i) {
      taskConfigs.add(taskProps);
    }
    return taskConfigs;
  }

  @Override
  public void stop() {
  }

  @Override
  public ConfigDef config() {
    return AwsLambdaSinkConnectorConfig.conf();
  }
}
